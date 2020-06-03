#pragma once

#define writel(value, addr) \
	*((volatile uint32_t *)addr) = value

#define clrbits_le32(addr, clear) \
    *((volatile uint32_t *)addr) &= ~clear

#define setbits_le32(addr, set) \
    *((volatile uint32_t *)addr) |= set

#define clrsetbits_le32(addr, clear, set) \
	*((volatile uint32_t *)addr) = (*((volatile uint32_t *)addr) & ~clear) | set
