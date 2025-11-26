pipeline {
    agent any 

    environment {
        // --- CONFIGURATION ---
        DOCKER_USER = "sammcaulay"
        DOCKER_IMAGE = "mathgame"
        VPS_IP = "132.145.45.36"
        
        // REPLACE THIS with your actual DuckDNS domain
        DOMAIN_NAME = "se3-mathgame.duckdns.org"
    }

    stages {
        stage('Build Docker Image') {
            steps {
                script {
                    echo "Building Docker image..."
                    sh "docker build -t ${DOCKER_USER}/${DOCKER_IMAGE}:latest -t ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER} ."
                }
            }
        }

        stage('Push to Hub') {
            steps {
                script {
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
                    // The command below is ONE single line to prevent syntax errors.
                    // It pulls the image, creates the network, cleans up old containers, and restarts both Game and Caddy.
                    sh "ssh -o StrictHostKeyChecking=no ubuntu@${VPS_IP} 'docker pull ${DOCKER_USER}/${DOCKER_IMAGE}:latest && docker network create game-net || true && docker stop mathgame caddy || true && docker rm mathgame caddy || true && docker run -d --name mathgame --network game-net --restart unless-stopped ${DOCKER_USER}/${DOCKER_IMAGE}:latest && docker run -d --name caddy --network game-net -p 80:80 -p 443:443 -v caddy_data:/data --restart unless-stopped caddy caddy reverse-proxy --from ${DOMAIN_NAME} --to mathgame:18080'"
                }
            }
        }
    }
    
    post {
        always {
            sh "docker rmi ${DOCKER_USER}/${DOCKER_IMAGE}:latest || true"
            sh "docker rmi ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER} || true"
        }
    }
}
