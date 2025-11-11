pipeline {
    // 1. Use 'any' agent. This will run on the main Jenkins machine
    // (your laptop's built-in node).
    agent any

    // 2. Define parameters
    // This will create a UI box in Jenkins when you click "Build",
    // asking you for the path to vcpkg.
    parameters {
        string(name: 'VCPKG_ROOT',
               // TODO: Change this to the *actual* path on your laptop
               defaultValue: '/home/sam/vcpkg',
               description: 'Absolute path to your vcpkg installation (e.g., /home/sam/vcpkg)')
    }

    // 3. Set environment variables for the build
    environment {
        // Use the parameter to build the full toolchain file path
        VCPKG_TOOLCHAIN = "${params.VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        BUILD_DIR = 'build'
    }

    // 4. Define the build stages
    stages {
        stage('Checkout') {
            steps {
                // Checkout the code from Git
                checkout scm
            }
        }

        stage('Configure') {
            steps {
                // Run CMake with the correct toolchain path
                // We also enable coverage flags
                sh "cmake -B ${env.BUILD_DIR} -S . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=${env.VCPKG_TOOLCHAIN} -DENABLE_COVERAGE=ON"
            }
        }

        stage('Build') {
            steps {
                // Build all targets (MathGame and RunTests)
                sh "cmake --build ${env.BUILD_DIR}"
            }
        }

        stage('Run Tests') {
            steps {
                // Run the compiled test executable
                sh "${env.BUILD_DIR}/RunTests"
            }
        }
    }

    // 5. Clean up at the end
    post {
        always {
            // Delete the 'build' directory to save space
            deleteDir()
            
            // cleanWs() is more thorough
            // cleanWs() 
        }
    }
}
