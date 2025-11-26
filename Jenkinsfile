pipeline {
    agent any 

    environment {
        // CHANGE 1: Point VCPKG_ROOT to the Jenkins workspace, NOT /home/sam
        VCPKG_ROOT = "${WORKSPACE}/vcpkg"
        BUILD_DIR = "build"
        DEPLOY_DIR = "/home/sam/MathGameDeploy"
    }

    stages {
        stage('Install Dependencies') {
            steps {
                dir('MathGameMain') {
                    script {
                        // CHANGE 2: Download vcpkg if it's not there yet
                        if (!fileExists("${VCPKG_ROOT}/vcpkg")) {
                            echo "Downloading fresh vcpkg for Jenkins..."
                            sh 'git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT}'
                            sh '${VCPKG_ROOT}/bootstrap-vcpkg.sh'
                        }
                        
                        // Now install dependencies using this LOCAL vcpkg
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
                    // Ensure the deploy directory exists and is writable
                    sh "mkdir -p ${DEPLOY_DIR}"
                    
                    echo "Deploying to ${DEPLOY_DIR}..."
                    sh 'pkill MathGame || true'
                    
                    sh "cp MathGameMain/${BUILD_DIR}/MathGame ${DEPLOY_DIR}/"
                    
                    sh "rm -rf ${DEPLOY_DIR}/static"
                    sh "cp -r MathGameMain/static ${DEPLOY_DIR}/"
                    
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
            // IMPORTANT: Do NOT clean workspace entirely, or vcpkg will re-download every time.
            // Only clean the build artifacts if you want to save space.
            dir('MathGameMain/build') {
                deleteDir()
            }
        }
    }
}
