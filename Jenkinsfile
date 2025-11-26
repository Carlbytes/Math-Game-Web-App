pipeline {
    agent any 

    environment {
        // --- SYSTEM CONFIG ---
        VCPKG_ROOT = "${WORKSPACE}/vcpkg"
        BUILD_DIR = "build"
        
        // --- DEPLOYMENT CONFIG ---
        DOCKER_USER = "sammcaulay"
        DOCKER_IMAGE = "mathgame"
        VPS_IP = "132.145.45.36"
        DOMAIN_NAME = "se3-mathgame.duckdns.org"
    }

    stages {
        // --- STAGE 1: DEPENDENCIES ---
        stage('Install Dependencies') {
            steps {
                dir('MathGameMain') {
                    script {
                        // Ensure vcpkg is present for the local test run
                        if (!fileExists("${VCPKG_ROOT}/vcpkg")) {
                            echo "Downloading vcpkg..."
                            sh 'git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT}'
                            sh '${VCPKG_ROOT}/bootstrap-vcpkg.sh'
                        }
                        sh '${VCPKG_ROOT}/vcpkg install'
                    }
                }
            }
        }

        // --- STAGE 2: BUILD (FOR TESTING) ---
        stage('Build (Local)') {
            steps {
                dir('MathGameMain') {
                    script {
                        // Configure and Build specifically for running tests
                        sh """
                            cmake -B ${BUILD_DIR} -S . \
                            -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
                            -DCMAKE_BUILD_TYPE=Debug
                        """
                        sh "cmake --build ${BUILD_DIR}"
                    }
                }
            }
        }

        // --- STAGE 3: UNIT TESTS ---
        stage('Unit Tests') {
            steps {
                dir('MathGameMain') {
                    dir("${BUILD_DIR}") {
                        script {
                            // Run the tests. If this fails, the pipeline STOPS here.
                            sh './RunTests -r junit -o results.xml'
                        }
                    }
                }
            }
            post {
                always {
                    // Report results to Jenkins graph
                    junit "MathGameMain/${BUILD_DIR}/results.xml"
                }
            }
        }

        // --- STAGE 4: PACKAGE (DOCKER) ---
        stage('Build Docker Image') {
            steps {
                script {
                    echo "Tests passed! Building production image..."
                    // Note: Docker will build its own clean version from scratch using the Dockerfile
                    sh "docker build -t ${DOCKER_USER}/${DOCKER_IMAGE}:latest -t ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER} ."
                }
            }
        }

        // --- STAGE 5: PUBLISH ---
        stage('Push to Hub') {
            steps {
                script {
                    withCredentials([usernamePassword(credentialsId: 'docker-hub-creds', passwordVariable: 'PASS', usernameVariable: 'USER')]) {
                        sh "echo $PASS | docker login -u $USER --password-stdin"
                        sh "docker push ${DOCKER_USER}/${DOCKER_IMAGE}:latest"
                        sh "docker push ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER}"
                    }
                }
            }
        }

        // --- STAGE 6: DEPLOY ---
        stage('Deploy to Oracle Cloud') {
            steps {
                sshagent(['oracle-vps-key']) {
                    // Single-line deploy command with Caddy for HTTPS
                    sh "ssh -o StrictHostKeyChecking=no ubuntu@${VPS_IP} 'docker pull ${DOCKER_USER}/${DOCKER_IMAGE}:latest && docker network create --driver bridge --opt com.docker.network.driver.mtu=1400 game-net || true && docker stop mathgame caddy || true && docker rm mathgame caddy || true && docker run -d --name mathgame --network game-net --restart unless-stopped ${DOCKER_USER}/${DOCKER_IMAGE}:latest && docker run -d --name caddy --network game-net -p 80:80 -p 443:443 -v caddy_data:/data --restart unless-stopped caddy caddy reverse-proxy --from ${DOMAIN_NAME} --to mathgame:18080'"
                }
            }
        }
    }
    
    post {
        always {
            // Clean up large artifacts
            dir('MathGameMain/build') { deleteDir() }
            sh "docker rmi ${DOCKER_USER}/${DOCKER_IMAGE}:latest || true"
            sh "docker rmi ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER} || true"
        }
    }
}
