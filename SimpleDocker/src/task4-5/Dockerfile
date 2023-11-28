FROM nginx:latest

WORKDIR /home/

COPY run.sh .
COPY server/server.c .
COPY server/nginx/nginx.conf /etc/nginx/

RUN apt-get update && apt-get install -y gcc spawn-fcgi libfcgi-dev; \
    useradd nginx; \
    chown -R nginx:nginx /etc/nginx/nginx.conf; \
    chown -R nginx:nginx /home/; \
    chown -R nginx:nginx /var/run/nginx.pid; \
    rm -rf /var/lib/apt/lists

USER nginx

ENTRYPOINT bash run.sh
HEALTHCHECK NONE