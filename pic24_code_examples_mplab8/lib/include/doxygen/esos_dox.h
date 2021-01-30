/*
 * "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
 * All rights reserved.
 * (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
 * (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
 * (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */

// Documentation placed on the ESOS page. This file only
// include comments for Doxygen; it does not contain any
// code.
/**
 * \page ESOS ESOS, the Embedded Systems Operating System
 * A portable, cooperative multitasking OS with tasks based on a very lightweight threading
 * system.
 *
 * \section ESOSCoreFiles Core files
 * Files which compose ESOS consist of:
 * - esos.c - contains the main ESOS scheduling loop
 * - esos.h - top-level ESOS header file
 * - esos_task.h - header file for defining ESOS tasks
 * - esos_comm.h - header file for defining ESOS communications services
 * - esos_comm.c - contains ESOS communications services library
 * - esos_irq.h - header file for defining ESOS interrupt services
 * - esos_irq.c - contains ESOS communications services library
 * - esos_pic24_tick.c - implements ESOS system tick for Microchip PIC24 Family implementation of ESOS
 * - esos_pic24_irq.h - implements ESOS interrupt services for Microchip PIC24 Family implementation of ESOS (header file)
 * - esos_pic24_irq.c - implements ESOS interrupt services for Microchip PIC24 Family implementation of ESOS (library code)
 * - esos_pic24_rs232.h - implements ESOS asychronous serial communications services for Microchip PIC24 Family implementation of ESOS (header file)
 * - esos_pic24_rs232.c - implements ESOS asychronous serial communications services for Microchip PIC24 Family implementation of ESOS (library code)
 * - esos_pic24_i2c.h - implements ESOS two-signal synchronous serial communications services for Microchip PIC24 Family implementation of ESOS (header file)
 * - esos_pic24_i2c.c - implements ESOS two-signal synchronous serial communications services for Microchip PIC24 Family implementation of ESOS (library code)
 * - esos_pic24_spi.h - implements ESOS three-signal synchronous serial communications services for Microchip PIC24 Family implementation of ESOS (header file)
 * - esos_pic24_spi.c - implements ESOS three-signal synchronous serial communications services for Microchip PIC24 Family implementation of ESOS (library code)
 * \section ESOSExampleFiles Example files
 * Example files which demonstrate ESOS operation include:
 * - esos_skel.c - A skeleton application which blinks the
 *   heartbeat LED using ESOS, providing a starting point for
 *   creating new ESOS applications.
 */
