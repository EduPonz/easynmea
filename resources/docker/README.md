# EasyNMEA Docker image

*EasyNMEA* ships a [Dockerfile](Dockerfile) to build an image that can be used to test the library's capabilities in an isolated environment.

* [Build docker image](#build-docker-image)
* [Run knowing the specific serial device](#run-knowing-the-specific-serial-device)
* [Run allowing for plug-unplug connectivity](#run-allowing-for-plug-unplug-connectivity)
* [Push image to DockerHub](#push-image-to-dockerhub)

## Build docker image

The image can be built with:

```bash
docker build -t easynmea -f Dockerfile .
```

## Run knowing the specific serial device

If your NMEA module is already connected to a serial port and it's not going to be unplugged, then you can just share that device with the container:

```bash
docker run -it --device=<path_to_device> easynmea bash
```

Then, inside the container, you can run the GPGGA example with:

```bash
/root/easynmea/build/examples/gpgga_example -b <baudrate> -p <path_to_device>
```

## Run allowing for plug-unplug connectivity

If your module may be unplug and plug while the container is running, you can still share the serial port with the Docker container by sharing all the devices of the same cgroup.
Plug your device and get the cgroup with:

```bash
ls -l <path_to_device> | awk '{print substr($5, 1, length($5)-1)}'
```

Then, run the container:

```bash
docker run -it -v /dev:/dev --device-cgroup-rule='c <device cgroup>:* rmw' easynmea bash
```

Finally, inside the container, you can run the GPGGA example as before:

```bash
/root/easynmea/build/examples/gpgga_example -b <baudrate> -p <path_to_device>
```

## Push image to DockerHub

First, you need to build the image.
Then, run:

```bash
docker tag easynmea eduponz/easynmea
docker push eduponz/easynmea
```

To be able to push the image, you'd need to be logged in in DockerHub:

```bash
docker login
```
