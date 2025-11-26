pipeline {
    agent any 

    environment {
        // --- CONFIGURATION ---
        DOCKER_USER = "sammcaulay"
        DOCKER_IMAGE = "mathgame"
        VPS_IP = "132.145.45.36"
        
        // REPLACE THIS with your actual DuckDNS (or other) domain
        // e.g., "sam-mathgame.duckdns.org"
        DOMAIN_NAME = "se3-mathgame.duckdns.org"
    }

    stages {
        stage('Build Docker Image') {
            steps {
                script {
                    echo "Building Docker image..."
                    // Builds the image using the Dockerfile in the root
                    sh "docker build -t ${DOCKER_USER}/${DOCKER_IMAGE}:latest -t ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER} ."
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
                sshagent(['oracle-vps-key']) {
                    sh """
                        ssh -o StrictHostKeyChecking=no ubuntu@${VPS_IP} '
                            # 1. Pull the latest image
                            docker pull ${DOCKER_USER}/${DOCKER_IMAGE}:latest && 
                            
                            # 2. Create a shared network (so Caddy can talk to the Game)
                            docker network create game-net || true && 
                            
                            # 3. Stop and remove old containers (ignore errors if they don't exist)
                            docker stop mathgame caddy || true && 
                            docker rm mathgame caddy || true && 
                            
                            # 4. Start the GAME container
                            # Notice: No ports (-p) are exposed to the public here. 
                            # It is hidden safely inside the "game-net" network.
                            docker run -d --name mathgame \\
                                --network game-net \\
                                --restart unless-stopped \\
                                ${DOCKER_USER}/${DOCKER_IMAGE}:latest && 

                            # 5. Start Caddy (The Secure Doorman)
                            # It listens on ports 80 & 443, handles SSL, and forwards valid traffic to "mathgame"
                            docker run -d --name caddy \\
                                --network game-net \\
                                -p 80:80 -p 443:443 \\
                                --restart unless-stopped \\
                                caddy caddy reverse-proxy --from ${DOMAIN_NAME} --to mathgame:18080'
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
