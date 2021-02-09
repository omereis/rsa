docker rm -f rest
docker build --rm -f rest.dockerfile -t rest .
docker run -it -d --name rest -p 5500:5500 rest
rem docker run -it -d --name docker_refsrv
docker exec -it rest bash