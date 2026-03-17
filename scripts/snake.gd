extends CharacterBody2D

@export var tile_map: TileMapLayer

@onready var design = $Sprite2D

var grid_size = 40
var move_delay = 0.5
var time_until_next_move = 0.0
var input_direction = Vector2.RIGHT
var is_moving = true
var prev_time_stamp = 0

func _physics_process(_delta):
	var new_input_dir = Vector2.ZERO
	if   Input.is_action_pressed("ui_up"):    new_input_dir = Vector2.UP
	elif Input.is_action_pressed("ui_down"):  new_input_dir = Vector2.DOWN
	elif Input.is_action_pressed("ui_left"):  new_input_dir = Vector2.LEFT
	elif Input.is_action_pressed("ui_right"): new_input_dir = Vector2.RIGHT

	if new_input_dir != Vector2.ZERO:
		input_direction = new_input_dir
		
	if time_until_next_move > 0:
		time_until_next_move -= _delta
		return
	else:
		$RayCast2D.target_position = input_direction * grid_size
		$RayCast2D.force_raycast_update()
		
		if $RayCast2D.is_colliding():
			print("fal!!! "  + $RayCast2D.get_collider().name)
		
		time_until_next_move = move_delay
		move_to_grid(input_direction)

func move_to_grid(dir):
	# log the time stamp
	# var new_time_stamp = Time.get_ticks_msec() / 1000.0
	# var delta = new_time_stamp - prev_time_stamp
	# prev_time_stamp = new_time_stamp
	# print("Timestamp: ", delta, " s")
	
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
	print("collecting...")
	var current_score = get_parent().get_node("CanvasLayer/Score")
	print("new score: " + str(int(current_score.text) + 1))
	current_score.text = str(int(current_score.text) + 1)
