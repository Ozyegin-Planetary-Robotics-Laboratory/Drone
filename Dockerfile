# Use an official ROS base image
FROM osrf/ros:noetic-desktop-full

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
    ros-noetic-joy \
    git \
    can-utils \
    inetutils-ping \
    net-tools \
    libncurses5-dev

# Create a workspace
RUN mkdir -p /ozurover/ares_ws/src

# Set the working directory
WORKDIR /ozurover/ares_ws

# Clone the necessary repositories
RUN git clone http://www.github.com/Ozyegin-Planetary-Robotics-Laboratory/ares_control.git src/ares_control
RUN git clone http://www.github.com/Ozyegin-Planetary-Robotics-Laboratory/deimos_control.git src/deimos_control

# Install TMotor CAN library (Assuming it has been cloned earlier)
RUN git clone http://www.github.com/Ozyegin-Planetary-Robotics-Laboratory/tmotorcan-cpp.git /ozurover/tmotorcan-cpp
RUN /bin/bash -c "source /opt/ros/noetic/setup.bash && cd /ozurover/tmotorcan-cpp && mkdir build && cd build && cmake .. && make install"

# Build the ROS workspace
RUN /bin/bash -c "source /opt/ros/noetic/setup.bash && catkin_make"

# Setup environment
RUN echo "source /ozurover/ares_ws/devel/setup.bash" >> ~/.bashrc
RUN echo "export ROS_HOSTNAME=motherbase" >> ~/.bashrc
RUN echo "export ROS_MASTER_URI=http://ares:11311" >> ~/.bashrc

# Entry point for the container
ENTRYPOINT [ "/ros_entrypoint.sh" ]
CMD [ "roslaunch", "ares_control", "locomotion.launch" ]
