#include "MappedStateMachine.hpp"

enum states {
	IDLE,
	ATTACK,
	RUN,
	JUMP,
	CLIMB,
	MOVE
};

// Example data structure
typedef struct playerData
{
	int animationId;
	bool invincible;
} data;

// Example processing codes for a node
int codefornode1(data a, data b) {
	return a.animationId > b.animationId;
}

int main() {

	MappedStateMachine<data> exampleStateMachine = MappedStateMachine<data>();

	exampleStateMachine.addNode(IDLE, data{1, false});

	exampleStateMachine.addNodeToParent(IDLE, MOVE, data{2, false}, 3, [](playerData, playerData) { return 5;});
	exampleStateMachine.addLink(IDLE, MOVE, &codefornode1, 4);
	exampleStateMachine.addNodeToParent(MOVE, JUMP, data{2, false}, 5, &codefornode1);
	exampleStateMachine.addLink(IDLE, JUMP, &codefornode1, 5);

	return 0;
}