pipeline {
    agent any 

    environment {
        // YOUR USERNAME
        DOCKER_USER = "sammcaulay"
        // YOUR REPO NAME
        DOCKER_IMAGE = "mathgame"
        // YOUR VPS IP
        VPS_IP = "132.145.45.36" 
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
                    // We use a single line here to avoid syntax errors with newlines
                    sh "ssh -o StrictHostKeyChecking=no ubuntu@${VPS_IP} 'docker pull ${DOCKER_USER}/${DOCKER_IMAGE}:latest && docker stop mathgame || true && docker rm mathgame || true && docker run -d --name mathgame -p 80:18080 --restart unless-stopped ${DOCKER_USER}/${DOCKER_IMAGE}:latest'"
                }
            }
        }
    }
    
    post {
        always {
            // Clean up local images to save space
            sh "docker rmi ${DOCKER_USER}/${DOCKER_IMAGE}:latest || true"
            sh "docker rmi ${DOCKER_USER}/${DOCKER_IMAGE}:${BUILD_NUMBER} || true"
        }
    }
}
