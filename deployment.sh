#!/bin/bash

# Build the Docker image
docker build -t authenticate_c .

# Run the Docker container
docker run -d -p 80:80 authenticate_c