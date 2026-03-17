extends CanvasLayer


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	hide()
	get_tree().paused = false
	Input.mouse_mode = Input.MOUSE_MODE_CAPTURED

func _input(event) -> void:
	if event.is_action_pressed("ui_cancel"):
		toggle_pause()
		
func toggle_pause():
	visible = !visible
	
	get_tree().paused = visible
	
	if visible:
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	else:
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED

func _on_quit_button_pressed() -> void:
	var current_score = get_parent().get_node("CanvasLayer/Score")
	Global.update_overall_high_score(int(current_score.text))
		
	get_tree().paused = false
	get_tree().change_scene_to_file("res://scenes/homepage.tscn")
