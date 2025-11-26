pipeline {
    agent any 

    environment {
        // REPLACE WITH YOUR USERNAME
        DOCKER_USER = "sammcaulay"
        DOCKER_IMAGE = "mathgame"
        // REPLACE WITH YOUR VPS IP
        VPS_IP = "132.145.45.36" 
    }

    stages {
        stage('Build Docker Image') {
            steps {
                script {
                    echo "Building Docker image from repository root..."
                    // The '.' at the end tells Docker to look in the CURRENT folder (Root)
                    // for the file named 'Dockerfile'.
                    sh "docker build -t ${DOCKER_USER}/${DOCKER_IMAGE}:latest -t ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER} ."
                }
            }
        }

        stage('Push to Hub') {
            steps {
                script {
                    // Log in to Docker Hub
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
                sshagent(['oracle-vps-key']) {
                    sh """
                        ssh -o StrictHostKeyChecking=no ubuntu@${VPS_IP} '
                            # 1. Pull the latest image
                            docker pull ${DOCKER_USER}/${DOCKER_IMAGE}:latest && \
                            
                            # 2. Stop the old game (if running)
                            docker stop mathgame || true && \
                            docker rm mathgame || true && \
                            
                            # 3. Run the new game
                            # Map port 80 (HTTP) to the game's 18080 so you can just type the IP
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
            // Clean up local images to save space on your laptop
            sh "docker rmi ${DOCKER_USER}/${DOCKER_IMAGE}:latest || true"
            sh "docker rmi ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER} || true"
        }
    }
}
