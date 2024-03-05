#include <crtdbg.h>
#include "arena.hpp"

struct Vector2D { float x, y; };
struct String { char str[16]{}; };
struct Transform { Vector2D position, size, velocity; float rotation; };
struct Object { String* name; Transform* transform; };

// Note that arena does not support dynamic array types

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	FixedArena<128> arena;

	Object* my_object = arena.Allocate<Object>();

	my_object->name = arena.Allocate<String>();
	strncpy_s(my_object->name->str, "My Object", sizeof(my_object->name->str));

	my_object->transform = arena.Allocate<Transform>();
	my_object->transform->position = { 0, 0 };
	my_object->transform->size = { 300, 300 };
	my_object->transform->velocity = { 30, 50 };
	my_object->transform->rotation = 0;

	arena.Flush();

	return 0;
}