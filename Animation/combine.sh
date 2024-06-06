#!/bin/bash

output_file="Video.mp4"
input_file="input.txt"

ffmpeg -f concat -safe 0 -i "$input_file" -c copy "$output_file"

