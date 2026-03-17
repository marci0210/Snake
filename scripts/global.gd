extends Node

var overall_high_score = 0

func update_overall_high_score(current_score: int) -> void:
	if Global.overall_high_score < current_score:
		Global.overall_high_score = current_score
	# todo: save it to somewhere

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
