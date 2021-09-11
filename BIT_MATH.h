/*
 * BIT_MATH.h
 *
 *  Created on: Aug 15, 2021
 *      Author: ibrahim
 */

#ifndef LIB_BIT_MATH_BIT_MATH_H_
#define LIB_BIT_MATH_BIT_MATH_H_


#define SET_BIT(REG,BIT)        REG |= (1 << BIT )
#define CLR_BIT(REG,BIT)        REG &= ~(1 << BIT )
#define TOGG_BIT(REG,BIT)       REG ^= (1 << BIT )
#define GET_BIT(REG,BIT)        ( (REG >> BIT) & 1)


#endif /* LIB_BIT_MATH_BIT_MATH_H_ */
