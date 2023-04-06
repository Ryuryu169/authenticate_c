#!/bin/bash

# Build the Docker image
docker build -t myapp .

# Run the Docker container
docker run -d -p 80:80 myapp