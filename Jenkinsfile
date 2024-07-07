pipeline {
    agent any
    environment {
        // 設置 Docker 映像的名稱
        DOCKER_IMAGE = 'turtle'
    }
    tools {
        // 使用你在 Global Tool Configuration 中配置的 Docker 工具名稱
        docker 'docker'
    }
    stages {
        stage('Checkout') {
            steps {
                // Checkout your code from version control
                // Replace the URL and credentials with your own
                git branch: 'main', url: 'https://github.com/fumchin/turtle.git'
            }
        }
        
        stage('Build') {
            steps {
                script {
                    // 動態構建 Docker 映像並命名
                    docker.build("${DOCKER_IMAGE}:latest")
                }
            }
        }
        
        stage('Test') {
            steps {
                // Run your tests here
                // Replace the command with your test command
                script {
                    // 使用剛剛構建的 Docker 映像來構建應用
                    docker.image("${DOCKER_IMAGE}:latest").inside {
                        sh 'make run'
                    }
                }
            }
        }

        // stage('Test') {
        //     steps {
        //         script {
        //             // 使用剛剛構建的 Docker 映像來運行測試
        //             docker.image("${DOCKER_IMAGE}:latest").inside {
        //                 sh 'make test'
        //             }
        //         }
        //     }
        // }
        
    }
}