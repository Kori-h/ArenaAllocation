#include <crtdbg.h>
#include "arena.hpp"

struct Vector2D { float x, y; };
struct Vector3D { float x, y, z; };
struct String { char str[16]{}; };
struct Transform { Vector2D position, size, velocity; float rotation; };
struct Object { String* name; Transform* transform; };

// Note that arena does not support dynamic array types

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	FixedArena arena(1024);

	int* my_int = arena.Allocate<int>();
	*my_int = -128;	

	float* my_float = arena.Allocate<float>();
	*my_float = 0.15f;

	// arena.Deallocate(my_int);
	// arena.Deallocate(my_float);

	// double* my_double = arena.Allocate<double>();
	
	// Vector3D* my_vector_3d = arena.Allocate<Vector3D>();

	// Object* my_object = arena.Allocate<Object>();
	// 
	// my_object->name = arena.Allocate<String>();
	// strncpy_s(my_object->name->str, "My Object", sizeof(my_object->name->str));
	// 
	// my_object->transform = arena.Allocate<Transform>();
	// my_object->transform->position = { 0, 0 };
	// my_object->transform->size = { 300, 300 };
	// my_object->transform->velocity = { 30, 50 };
	// my_object->transform->rotation = 0;

	arena.Flush();

	return 0;
}