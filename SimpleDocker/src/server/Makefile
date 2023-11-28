all: build_docker

build_docker:
	sudo docker run --name c_server -d -p 81:81 -it nginx bash
	sudo docker exec c_server apt-get update
	sudo docker exec c_server apt-get install -y gcc spawn-fcgi libfcgi-dev
	sudo docker cp server.c c_server:/home
	sudo docker cp nginx/nginx.conf c_server:/etc/nginx/
	sudo docker exec c_server gcc /home/server.c -lfcgi -o /home/server
	sudo docker exec -it c_server spawn-fcgi -p 8080 /home/server
	sudo docker exec -it c_server service nginx start