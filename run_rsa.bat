docker rm -f rsa
touch nosource.cpp
docker build --rm -f rsa.dockerfile -t rsa .
docker run -it -d -h rsa --name rsa -p 5500:5500 rsa
rem docker run -it -d --name docker_refsrv
docker exec -it rsa bash
del nosource.cpp