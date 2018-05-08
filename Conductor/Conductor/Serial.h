//
// Created by Lilli Szafranski on 5/7/18.
//

#ifndef VIRROR_CONDUCTOR_SERIAL_H
#define VIRROR_CONDUCTOR_SERIAL_H

/* Returning -1 is bad I think */
int	write_data(int fileDescriptor, const void *data, size_t size);
int	read_data(int fileDescriptor, void *data, size_t size);

/* Returning -1 is bad I think */
int new_serial_device(const char *devName);

#endif //VIRROR_CONDUCTOR_SERIAL_H
