
pipeline {
    agent any 

    environment {
        // Point to a local folder INSIDE the workspace
        VCPKG_ROOT = "${WORKSPACE}/vcpkg"
        BUILD_DIR = "build" 
    }

    stages {
        stage('Setup Dependencies') {
            steps {
                dir('MathGameMain') {
                    script {
                        // Check if vcpkg is missing and download it if needed
                        if (!fileExists("${VCPKG_ROOT}/vcpkg")) {
                            echo "Downloading vcpkg..."
                            sh 'git clone https://github.com/microsoft/vcpkg.git ${VCPKG_ROOT}'
                            sh '${VCPKG_ROOT}/bootstrap-vcpkg.sh'
                        }
                        // Install dependencies
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
        
        stage('Run Game (Verify)') {
            steps {
                dir('MathGameMain') {
                    dir("${BUILD_DIR}") {
                        script {
                            sh 'nohup ./MathGame > server.log 2>&1 & echo $! > pid.file'
                            sleep 5
                            sh 'curl -v --fail http://localhost:18080/'
                            sh 'kill $(cat pid.file)'
                        }
                    }
                }
            }
        }
    }
    // Note: cleanWs() is removed so vcpkg stays cached between builds
}
