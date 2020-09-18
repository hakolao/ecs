# LibECS
An entity component system written in C using a limited number of c libraries.
Only `malloc`, `va_start`, `va_end`, `va_arg` are used except for the `demo`.

Demo uses SDL2.

## Run
```
git clone https://github.com/hakolao/ecs.git && cd ecs
make demo # runs a visual demo, if you wish to edit parameters, go to demo/include/demo.h
make test # runs tests
```

## Use
```
make # creates libecs.a that you an include in your libs
```

## Library usage

`demo/` should give you an idea how this works, but here is a short guide:

Structs & enums must be defined to be used by the library functions. At least
component ids & corresponding components, and system ids.
```c
	/*
	** Demo components (to be used by each entity)
	*/

	typedef struct s_position
	{
		float x;
		float y;
	} t_position;

	typedef struct s_velocity
	{
		float dx;
		float dy;
	} t_velocity;

	typedef struct t_render
	{
		uint32_t color;
		uint32_t width;
		uint32_t height;
		t_window **window;
	} t_render;

	/*
	** Component identifiers, should be powers of 2 and ULL for valid component
	** ids.
	*/

	typedef enum e_comp_id
	{
		comp_empty = 0ULL,
		comp_pos = 1ULL,
		comp_vel = 1ULL << 1,
		comp_render = 1ULL << 2,
	} t_comp_id;

	/*
	** System ids.
	** If a system should not be run with other systems, its id should not be 0
	*/

	typedef enum e_system_id
	{
		system_render = 123,
		system_move = 111
	} t_system_id;
```

1. Create world initializes the world struct.

```c
t_ecs_world *world = ecs_world_create("Demo world", MAX_ENTITIES);
```

2. Create systems which must include components the system uses and system
handle functions. See `demo/demo_system.c` on what the render handles could look
like.

```c
	static void system_movement_handle(t_ecs_world *world, uint64_t entity_index)
	{
		void *system_args;
		t_position *pos;
		t_velocity *vel;
		uint32_t dt;

		system_args = world->systems[ecs_system_index(world, system_move)].params;
		if (system_args != NULL)
		{
			dt = (uint32_t)(*(size_t*)system_args);
			pos = (t_position*)hash_map_get(ecs_component_entities(world, comp_pos),
				entity_index);
			vel = (t_velocity*)hash_map_get(ecs_component_entities(world, comp_vel),
				entity_index);
			if (pos && vel)
			{
				pos->x += vel->dx * dt;
				pos->y += vel->dy * dt;
			}
		}
	}

	ecs_world_system_add(app->world, (t_system){
		.system_id = system_render,
		.components_mask = comp_render | comp_pos,
		.system_handle_func = system_render_handle,
		.params = NULL
	});
	ecs_world_system_add(app->world, (t_system){
		.system_id = system_move,
		.components_mask = comp_pos | comp_vel,
		.system_handle_func = system_movement_handle,
		.params = (void*)&app->delta_time
	});
```

3. Create entities with initial components they have. Components can be added
afterwards as well.

```c
void entities_create(t_app *app)
{
	int32_t i;
	int32_t size;

	i = 1;
	while (app->world->num_entities < MAX_ENTITIES)
	{
		size = 3;
		ecs_world_entity_add(app->world, 3,
			&(t_component){.id = comp_pos, .size = sizeof(t_position),
				.data = &(t_position){.x = i,.y = 1}},
			&(t_component){.id = comp_vel, .size = sizeof(t_velocity),
				.data = &(t_velocity){.dx = 1, .dy = 1}},
			&(t_component){.id = comp_render, .size = sizeof(t_render),
				.data = &(t_render){.color = i, .width = size,
					.height = size, .window = &app->window}});
		i = ft_rand(i);
	}
}
```

4. Run Systems (this would usually happen each game loop, but can be run only once too)

```c
	ecs_systems_run(app->world, system_move | system_render);
```

5. Update system params if needed, e.g. delta time. Though before ecs_systems_run.
```c
	ecs_system_update_params(app->world, system_move, &app->delta_time);
```
