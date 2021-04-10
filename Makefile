scribbler:
	gcc \
		src/main.c \
		src/objects/graphics.c \
		src/objects/game.c \
		src/objects/room.c \
		src/ECS/Components.c \
		src/ECS/ECS.c \
		src/ECS/MoveComponent.c \
		src/ECS/SpriteComponent.c \
		src/ECS/MappedComponent.c \
		src/ECS/JourneyComponent.c \
		src/ECS/AnimateComponent.c \
		src/ECS/ClickComponent.c \
		src/solver.c \
		-lSDL2main \
		-lSDL2 \
		-lSDL2_image \
		-ldl \
		-lm \
		-Wall \
		-o \
	scribbler.bin
