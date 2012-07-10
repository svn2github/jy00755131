#ifndef _Parameter_H
#define _Parameter_H

// 定义震源个数上限
#define ActiveSourceNumLimit	10
// 数据处理对象类型
#define PROCESS_IMPULSIVE		0		// 脉冲
#define PROCESS_STACKIMPULSIVE	1		// 脉冲叠加
#define PROCESS_CORRELAFTER		2		// 叠加后相关
#define PROCESS_CORRELBEFORE	3		// 叠加前相关
#define PROCESS_STACK			4		// 可控震源地震叠加
// 处理设置选项
#define PROCESS_STANDARD		0		// 标准
#define PROCESS_ADVANCE			1		// 高级
#endif