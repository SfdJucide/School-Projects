#!/bin/bash

export DOCKER_CONTENT_TRUST=1
sudo docker build -t hehe:1.0 .
sudo docker run --name roi -p 80:81 -dit -v $(pwd)/server/nginx/nginx.conf:/etc/nginx/nginx.conf hehe:1.0