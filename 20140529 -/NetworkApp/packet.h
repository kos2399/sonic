#pragma once

enum packet_type
{
	PT_NULL, // not used
	PT_STR,
	PT_DUMMY,
};

struct packet
{
	unsigned char btType;
	unsigned char cbSize[2];
	unsigned char* data;
};

