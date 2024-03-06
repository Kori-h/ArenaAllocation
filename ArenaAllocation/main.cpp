#include <crtdbg.h>
#include "arena.hpp"
#include "arenallist.hpp"

struct Vector2D					{ float x, y; };
struct String					{ char str[16]{}; };

struct Component				{ bool active; size_t id; };
struct Transform : Component	{ Vector2D position, size, velocity; float rotation; };
struct Object					{ String* name; ArenaList<Component*>* components; };

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/******************************************/
	/* create arena
	/******************************************/

	FixedArena arena(128);

	/******************************************/
	/* test allocation for primitive types
	/******************************************/

	int* my_int = arena.allocate<int>();
	*my_int = 3750;	
	
	float* my_float = arena.allocate<float>();
	*my_float = 0.00175f;
	
	double* my_double = arena.allocate<double>();
	*my_double = -0.00000125;

	long long int* my_long_long_int = arena.allocate<long long int>();
	*my_long_long_int = 51325875000ll;
	
	arena.deallocate(my_int);
	arena.deallocate(my_float);
	arena.deallocate(my_double);
	arena.deallocate(my_long_long_int);

	// test double deallocation
	arena.deallocate(my_long_long_int);

	/******************************************/
	/* test allocation for user - defined lists
	/******************************************/

	ArenaList<int>* my_int_list = arena.allocate<ArenaList<int>>();

	my_int_list->setArena(arena);
	my_int_list->pushBack(10);

	my_int_list->clear();
	arena.deallocate(my_int_list);

	/******************************************/
	/* test allocation for objects and components
	/******************************************/

	Object* my_object = arena.allocate<Object>();
	
	my_object->name = arena.allocate<String>();
	strncpy_s(my_object->name->str, "My Object", sizeof(my_object->name->str));
	
	my_object->components = arena.allocate<ArenaList<Component*>>();
	my_object->components->setArena(arena);
	
	Transform* transform = arena.allocate<Transform>();
	
	transform->position = { 0, 0 };
	transform->size = { 300, 300 };
	transform->velocity = { 30, 50 };
	transform->rotation = 0;
	
	// push Transform into component list
	my_object->components->pushBack(static_cast<Component*>(transform));
	
	// disgusting syntax but its proof of concept
	Transform* tf = static_cast<Transform*>(*(*my_object->components)[0]);
	
	// prints transform data
	std::cout << std::endl;
	std::cout << std::setw(12) << std::left << "Name: "  << my_object->name->str << std::endl;
	std::cout << std::setw(12) << std::left << "Position: " << tf->position.x << ", " << tf->position.y << std::endl;
	std::cout << std::setw(12) << std::left << "Size: " << tf->size.x << ", " << tf->size.y << std::endl;
	std::cout << std::setw(12) << std::left << "Velocity: " << tf->velocity.x << ", " << tf->velocity.y << std::endl;
	std::cout << std::setw(12) << std::left << "Rotation: " << tf->rotation << std::endl;
	std::cout << std::endl;

	arena.flush();

	return 0;
}