#pragma once


class Player
{
private:
	float playerSpeed = 5.0f;
	float gravity = -9.8f;
	float playerHeight = 1.8f;
	float jumpForce = 5.0f;
	float verticalVelocity = 0.0f;
	bool isGrounded = false;
};