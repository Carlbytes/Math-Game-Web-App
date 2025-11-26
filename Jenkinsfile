pipeline {
    agent any 

    environment {
        VCPKG_ROOT = "${WORKSPACE}/vcpkg"
        BUILD_DIR = "build"
        // We will still deploy here
        DEPLOY_DIR = "/home/sam/MathGameDeploy"
    }

    stages {
        stage('Install Dependencies') {
            steps {
                dir('MathGameMain') {
                    script {
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
                    
                    // Ensure the directory exists (using -p so it doesn't fail if it exists)
                    sh "mkdir -p ${DEPLOY_DIR}"
                    
                    // 1. Kill existing server
                    sh 'pkill MathGame || true'
                    
                    // 2. Copy files
                    // We use full paths here so we don't need to change directories context
                    sh "cp MathGameMain/${BUILD_DIR}/MathGame ${DEPLOY_DIR}/"
                    sh "rm -rf ${DEPLOY_DIR}/static"
                    sh "cp -r MathGameMain/static ${DEPLOY_DIR}/"
                    
                    // 3. Start the server safely
                    // We use 'cd' INSIDE the sh command to avoid the permission error
                    withEnv(['JENKINS_NODE_COOKIE=dontKillMe']) {
                        sh "cd ${DEPLOY_DIR} && nohup ./MathGame > game_log.txt 2>&1 &"
                    }
                }
            }
        }
    }
    
    post {
        always {
            // Optional: Clean build artifacts to save space, but keep vcpkg
            dir('MathGameMain/build') {
                deleteDir()
            }
        }
    }
}
