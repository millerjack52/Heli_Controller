/*
 * rotors.h
 *
 *  Created on: 7/05/2024
 *      Author: jmi145
 */

#ifndef ROTORS_H_
#define ROTORS_H_

void initialisePWM (void);
void initialisePWM2 (void);
void setPWM (uint32_t u32Freq, uint32_t u32Duty);
void setPWM2 (uint32_t u32Freq, uint32_t u32Duty);
int32_t controlTailGoal (int32_t targetYawDeg);
int32_t controlMainGoal (int32_t targetAlt);
void initRotors(void);



#endif /* ROTORS_H_ */
