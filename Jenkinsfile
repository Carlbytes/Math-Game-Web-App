pipeline {
    agent any 

    environment {
        // Path to vcpkg on your Linux Mint machine
        VCPKG_ROOT = "/home/sam/vcpkg" 
        // The build folder will be created inside MathGameMain
        BUILD_DIR = "build" 
    }

    stages {
        stage('Debug Info') {
            steps {
                script {
                    // This helps us verify where we are
                    echo "Root Directory: ${pwd()}"
                    sh 'ls -F'
                }
            }
        }

        stage('Install Dependencies') {
            steps {
                // Step into the folder containing vcpkg.json
                dir('MathGameMain') {
                    script {
                        echo "Installing dependencies in ${pwd()}"
                        sh '${VCPKG_ROOT}/vcpkg install'
                    }
                }
            }
        }

        stage('Configure') {
            steps {
                // Step into the folder containing CMakeLists.txt
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
                    // Run the tests inside the build directory
                    dir("${BUILD_DIR}") {
                        script {
                            // This runs the 'RunTests' executable built by CMake
                            sh './RunTests -r junit -o results.xml'
                        }
                    }
                }
            }
            post {
                always {
                    // Jenkins needs the path relative to the root to find the XML
                    junit "MathGameMain/${BUILD_DIR}/results.xml"
                }
            }
        }
        
        stage('Run Game (Verify)') {
            steps {
                dir('MathGameMain') {
                    dir("${BUILD_DIR}") {
                        script {
                            // 1. Start the game server in the background
                            // 2. Save its Process ID (PID) to a file so we can kill it later
                            sh 'nohup ./MathGame > server.log 2>&1 & echo $! > pid.file'
                            
                            // 3. Wait 5 seconds for the server to start
                            sleep 5
                            
                            // 4. Test if the login page loads (HTTP 200 OK)
                            sh 'curl -v --fail http://localhost:18080/'
                            
                            // 5. Kill the server using the PID we saved
                            sh 'kill $(cat pid.file)'
                        }
                    }
                }
            }
        }
    }
    
    post {
        always {
            // Clean up the workspace to save space
            cleanWs()
        }
    }
}
