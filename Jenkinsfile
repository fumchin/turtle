pipeline {
    agent any
    
    stages {
        stage('Checkout') {
            steps {
                // Checkout your code from version control
                // Replace the URL and credentials with your own
                git branch: 'main', url: 'https://github.com/your-repo.git', credentialsId: 'your-credentials-id'
            }
        }
        
        stage('Build') {
            steps {
                echo 'Building the project'
                sh 'make all'
            }
        }
        
        stage('Test') {
            steps {
                // Run your tests here
                // Replace the command with your test command
                echo 'Running tests'
                sh 'make run'
            }
        }
        
    }
}