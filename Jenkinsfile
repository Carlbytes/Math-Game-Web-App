pipeline {
    agent any 

    environment {
        VCPKG_ROOT = "${WORKSPACE}/vcpkg"
        BUILD_DIR = "build"
       
        // --- CONFIGURATION ---
        // Your Docker Hub Username
        DOCKER_USER = "sammcaulay"
        // Your Docker Hub Repo Name
        DOCKER_IMAGE = "mathgame"
        // Your Oracle VPS IP Address
        VPS_IP = "132.145.45.36" 
    }

    stages {
        stage('Install Dependencies') {
            steps {
                dir('MathGameMain') {
                    script {
                        if (!fileExists("${VCPKG_ROOT}/vcpkg")) {
                            sh 'git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT}'
                            sh '${VCPKG_ROOT}/bootstrap-vcpkg.sh'
                        }
                        sh '${VCPKG_ROOT}/vcpkg install'
                    }
                }
            }
        }

        stage('Configure & Build (Local)') {
            steps {
                dir('MathGameMain') {
                    script {
                        sh """
                            cmake -B ${BUILD_DIR} -S . \
                            -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
                            -DCMAKE_BUILD_TYPE=Release
                        """
                        sh "cmake --build ${BUILD_DIR}"
                    }
                }
            }
        }

        stage('Build Docker Image') {
            steps {
                script {
                    // Build the image using the Dockerfile in MathGameMain
                    // We tag it with the build number so every build is unique
                    sh "docker build -t ${DOCKER_USER}/${DOCKER_IMAGE}:latest -t ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER} -f MathGameMain/Dockerfile MathGameMain"
                }
            }
        }

        stage('Push to Hub') {
            steps {
                script {
                    // Log in to Docker Hub using the credentials we stored earlier
                    withCredentials([usernamePassword(credentialsId: 'docker-hub-creds', passwordVariable: 'PASS', usernameVariable: 'USER')]) {
                        sh "echo $PASS | docker login -u $USER --password-stdin"
                        sh "docker push ${DOCKER_USER}/${DOCKER_IMAGE}:latest"
                        sh "docker push ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER}"
                    }
                }
            }
        }

        stage('Deploy to Oracle Cloud') {
            steps {
                // SSH into the remote server and run the game
                sshagent(['oracle-vps-key']) {
                    sh """
                        ssh -o StrictHostKeyChecking=no ubuntu@${VPS_IP} '
                            # 1. Pull the latest image
                            docker pull ${DOCKER_USER}/${DOCKER_IMAGE}:latest && \
                            
                            # 2. Stop and remove the old game container (ignore errors if not running)
                            docker stop mathgame || true && \
                            docker rm mathgame || true && \
                            
                            # 3. Start the new container
                            # -d: Detached mode
                            # --restart unless-stopped: If the server reboots, the game auto-starts
                            # -p 80:18080: Map external port 80 (standard HTTP) to internal 18080
                            docker run -d --name mathgame \
                                -p 80:18080 \
                                --restart unless-stopped \
                                ${DOCKER_USER}/${DOCKER_IMAGE}:latest'
                    """
                }
            }
        }
    }
    
    post {
        always {
            // Clean up local docker images to save space on your laptop
            sh "docker rmi ${DOCKER_USER}/${DOCKER_IMAGE}:latest || true"
            sh "docker rmi ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER} || true"
        }
    }
}
