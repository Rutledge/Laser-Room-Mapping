/* 
 * File:   operations.h
 * Author: Rutledge
 *
 * Created on November 30, 2013, 11:53 PM
 */

#ifndef OPERATIONS_H
#define	OPERATIONS_H

void wait_onesec();
void move_motor();
void laser_init();
unsigned long laser_read();
void wifi_write(unsigned long total);

#endif	/* OPERATIONS_H */

