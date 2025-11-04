#include <iostream>
#include <smmo_net.h>

enum class CustomMsgTypes : uint32_t
{
	FireProjectile,
	MovePlayer
};

int main()
{
	smmo::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::FireProjectile;

	int a = 1;
	bool b = true;
	float c = 3.14159f;

	struct
	{
		float x;
		float y;
	} d[5];

	msg << a << b << c << d;

	a = 99;
	b = false;
	c = 99.0f;

	msg >> d >> c >> b >> a;

	return 0;
}