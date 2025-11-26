
pipeline {
    agent any 

    environment {
        VCPKG_ROOT = "/home/sam/vcpkg"
        BUILD_DIR = "build"
        DEPLOY_DIR = "/home/sam/MathGameDeploy"
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

        stage('Configure') {
            steps {
                dir('MathGameMain') {
                    script {
                        sh """
                            cmake -B ${BUILD_DIR} -S . \
                            -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
                            -DCMAKE_BUILD_TYPE=Release \
                            -DENABLE_COVERAGE=OFF
                        """
                        // Note: I switched to 'Release' mode for better performance in deployment
                    }
                }
            }
        }

        stage('Build') {
            steps {
                dir('MathGameMain') {
                    script {
                        sh "cmake --build ${BUILD_DIR}"
                    }
                }
            }
        }

        stage('Unit Tests') {
            steps {
                dir('MathGameMain') {
                    dir("${BUILD_DIR}") {
                        script {
                            sh './RunTests -r junit -o results.xml'
                        }
                    }
                }
            }
            post {
                always {
                    junit "MathGameMain/${BUILD_DIR}/results.xml"
                }
            }
        }
        
        stage('Deploy Locally') {
            steps {
                script {
                    echo "Deploying to ${DEPLOY_DIR}..."
                    
                    // 1. Kill existing server if running (ignore error if not running)
                    sh 'pkill MathGame || true'
                    
                    // 2. Copy the new Executable
                    sh "cp MathGameMain/${BUILD_DIR}/MathGame ${DEPLOY_DIR}/"
                    
                    // 3. Copy the Static folder (CSS/HTML)
                    // We remove the old static folder first to ensure no stale files remain
                    sh "rm -rf ${DEPLOY_DIR}/static"
                    sh "cp -r MathGameMain/static ${DEPLOY_DIR}/"
                    
                    // 4. Start the server in the background detached from Jenkins
                    // JENKINS_NODE_COOKIE=dontKillMe tells Jenkins "Leave this process alone"
                    dir("${DEPLOY_DIR}") {
                        withEnv(['JENKINS_NODE_COOKIE=dontKillMe']) {
                            sh 'nohup ./MathGame > game_log.txt 2>&1 &'
                        }
                    }
                }
            }
        }
    }
    
    post {
        always {
            cleanWs()
        }
    }
}

