build:
	docker build -t scribbler-build .
run: build
	mkdir -p output
	docker run --name scribbler-build --rm --mount type=bind,source=$(shell pwd)/output,target=/output scribbler-build
	chown $(shell logname) output output/scribbler

