pipeline {
    agent any 

    environment {
        VCPKG_ROOT = "${WORKSPACE}/vcpkg"
        BUILD_DIR = "build"
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
                        // CHANGE 1 & 2: Switch to Debug and Enable Coverage
                        sh """
                            cmake -B ${BUILD_DIR} -S . \
                            -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake \
                            -DCMAKE_BUILD_TYPE=Debug \
                            -DENABLE_COVERAGE=ON
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

        stage('Unit Tests & Coverage') {
            steps {
                dir('MathGameMain') {
                    dir("${BUILD_DIR}") {
                        script {
                            // 1. Run the tests (this generates the raw .gcda coverage files)
                            sh './RunTests -r junit -o results.xml'
                            
                            // 2. Run gcovr to create the XML report
                            // -r .. : Sets the root to the parent directory (MathGameMain)
                            // --xml : Output in Cobertura XML format
                            sh 'gcovr -r .. --xml -o coverage.xml'
                        }
                    }
                }
            }
            post {
                always {
                    // Publish Test Results
                    junit "MathGameMain/${BUILD_DIR}/results.xml"
                    
                    // CHANGE 3: Publish Coverage Report
                    // This requires the 'Code Coverage API' plugin
                    recordCoverage(tools: [[parser: 'COBERTURA', pattern: "MathGameMain/${BUILD_DIR}/coverage.xml"]])
                }
            }
        }
        
        stage('Deploy Locally') {
            steps {
                script {
                    echo "Deploying to ${DEPLOY_DIR}..."
                    sh "mkdir -p ${DEPLOY_DIR}"
                    sh 'pkill MathGame || true'
                    sh "cp MathGameMain/${BUILD_DIR}/MathGame ${DEPLOY_DIR}/"
                    sh "rm -rf ${DEPLOY_DIR}/static"
                    sh "cp -r MathGameMain/static ${DEPLOY_DIR}/"
                    
                    dir("${DEPLOY_DIR}") {
                        withEnv(['JENKINS_NODE_COOKIE=dontKillMe']) {
                            sh "cd ${DEPLOY_DIR} && nohup ./MathGame > game_log.txt 2>&1 &"
                        }
                    }
                }
            }
        }
    }
    
    post {
        always {
            // Optional: Clean build artifacts but keep vcpkg
            dir('MathGameMain/build') {
                deleteDir()
            }
        }
    }
}