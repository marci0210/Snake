extends CharacterBody2D

@export var tile_map: TileMapLayer

@onready var design = $Sprite2D

var grid_size = 40
var move_delay = 0.5
var time_until_next_move = 0.0
var current_direction = Vector2.RIGHT
var is_moving = true
var prev_time_stamp = 0

func _physics_process(_delta):
	var new_input_dir = Vector2.ZERO
	if   Input.is_action_pressed("ui_up") and current_direction != Vector2.DOWN:    
		new_input_dir = Vector2.UP
	elif Input.is_action_pressed("ui_down") and current_direction != Vector2.UP:  
		new_input_dir = Vector2.DOWN
	elif Input.is_action_pressed("ui_left") and current_direction != Vector2.RIGHT:  
		new_input_dir = Vector2.LEFT
	elif Input.is_action_pressed("ui_right") and current_direction != Vector2.LEFT: 
		new_input_dir = Vector2.RIGHT

	if new_input_dir != Vector2.ZERO:
		current_direction = new_input_dir
		
	if time_until_next_move > 0:
		time_until_next_move -= _delta
		return
	else:
		$RayCast2D.target_position = current_direction * grid_size
		$RayCast2D.force_raycast_update()
		
		# in case of collision, update max score then exit
		if $RayCast2D.is_colliding():
			var current_score = get_parent().get_node("CanvasLayer/Score")
			Global.update_overall_high_score(int(current_score.text))
			
			Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
			get_tree().change_scene_to_file("res://scenes/homepage.tscn")
		
		time_until_next_move = move_delay
		move_to_grid(current_direction)

func move_to_grid(dir):
	if dir == Vector2.UP:
		design.rotation_degrees = 90
	elif dir == Vector2.DOWN:
		design.rotation_degrees = -90
	elif dir == Vector2.LEFT:
		design.rotation_degrees = 0
	elif dir == Vector2.RIGHT:
		design.rotation_degrees = -180
		
	var target_pos = position + (dir * grid_size)
	position = target_pos

func _on_area_2d_body_entered(body: Node2D) -> void:
	# increase current score
	var current_score = get_parent().get_node("CanvasLayer/Score")
	current_score.text = str(int(current_score.text) + 1)
	
	# generate new coordiante to the object
	var object = get_parent().get_node("Area2D")
	object.position = generate_random_grid_position()
	
func generate_random_grid_position():
	var random_x = randi_range(1, 18) 
	var random_y = randi_range(1, 18)
	
	var new_pos = Vector2((random_x * grid_size) + 20, (random_y * grid_size) + 20)
	
	return new_pos
	
