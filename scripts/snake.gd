extends CharacterBody2D

@export var tile_map: TileMapLayer
@export var body_segment_scene: PackedScene

@onready var design = $Sprite2D_head

var body_segments: Array = []

var grid_size = 40
var move_delay = 0.5
var time_until_next_move = 0.0

var current_direction = Vector2.RIGHT
var new_target_direction = Vector2.ZERO

var is_moving = true
var prev_time_stamp = 0

@onready var curve_texture = preload("res://assets/textures/snake_curve.png")
@onready var body_texture = preload("res://assets/textures/snake_body.png")

func _ready():
	$RayCast2D.target_position = current_direction * grid_size
	$RayCast2D.force_raycast_update()
	
	body_segments.append(self)
	add_initial_segments()
	add_initial_segments()
	add_initial_segments()
	add_initial_segments()
	add_initial_segments()
	add_initial_segments()

func add_initial_segments() -> void:
	var new_segment = body_segment_scene.instantiate()
	var last_segment = body_segments[-1]
	if current_direction == Vector2.RIGHT:
		new_segment.position = last_segment.position + Vector2(-grid_size, 0)
	elif current_direction == Vector2.LEFT:
		new_segment.position = last_segment.position + Vector2(grid_size, 0)
	elif current_direction == Vector2.UP:
		new_segment.position = last_segment.position + Vector2(0, -grid_size)
	elif current_direction == Vector2.DOWN:
		new_segment.position = last_segment.position + Vector2(0, grid_size)
	new_segment.rotation = last_segment.rotation
	get_parent().add_child.call_deferred(new_segment)
	body_segments.append(new_segment)

func _physics_process(_delta):
	if   Input.is_action_pressed("ui_up") and current_direction != Vector2.DOWN:    
		new_target_direction = Vector2.UP
	elif Input.is_action_pressed("ui_down") and current_direction != Vector2.UP:  
		new_target_direction = Vector2.DOWN
	elif Input.is_action_pressed("ui_left") and current_direction != Vector2.RIGHT:  
		new_target_direction = Vector2.LEFT
	elif Input.is_action_pressed("ui_right") and current_direction != Vector2.LEFT: 
		new_target_direction = Vector2.RIGHT

		
	if time_until_next_move > 0:
		time_until_next_move -= _delta
		return
	else:
		if new_target_direction != Vector2.ZERO:
			current_direction = new_target_direction
			new_target_direction = Vector2.ZERO
		
		# rotate the segments
		rotate_segments(current_direction)
		
		# in case of collision, update max score then exit
		$RayCast2D.force_raycast_update()
		if $RayCast2D.is_colliding():
			var collider = $RayCast2D.get_collider()
	
			if collider is TileMapLayer or $RayCast2D.get_collider().collision_layer == 1: # wall or snake itself
				var current_score = get_parent().get_node("CanvasLayer/Score")
				Global.update_overall_high_score(int(current_score.text))
				
				Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
				get_tree().change_scene_to_file("res://scenes/homepage.tscn")
			elif $RayCast2D.get_collider().collision_layer == 2: # object
				pick_up_object()
		
		# then move the segments
		move_to_grid(current_direction)
		
		time_until_next_move = move_delay
		
func rotate_segments(dir):
	for i in range(body_segments.size() - 1, 0, -1):
		body_segments[i].rotation = body_segments[i-1].rotation
			
	rotation = dir.angle()
	
	for i in range(body_segments.size() - 1, 0, -1):
		if body_segments[i].rotation != body_segments[i-1].rotation:
			body_segments[i].get_node("Sprite2D").set_texture(curve_texture)
			var degree = angle_difference(body_segments[i].rotation, body_segments[i-1].rotation)
			if is_equal_approx(degree, -PI/2):
				body_segments[i].get_node("Sprite2D").flip_v = true
			else:
				body_segments[i].get_node("Sprite2D").flip_v = false
		else:
			body_segments[i].get_node("Sprite2D").set_texture(body_texture)

func move_to_grid(dir):		
	for i in range(body_segments.size() - 1, 0, -1):
		body_segments[i].position = body_segments[i-1].position
		
	var target_pos = position + (dir * grid_size)
	position = target_pos

func pick_up_object() -> void:
	# increase current score
	var current_score = get_parent().get_node("CanvasLayer/Score")
	current_score.text = str(int(current_score.text) + 1)
	
	# add new segment to the Snake
	add_initial_segments()
	
	# generate new coordiante to the object
	var object = get_parent().get_node("ObjectArea2D")
	object.position = generate_random_grid_position()
	
func generate_random_grid_position():
	var random_x = randi_range(1, 18) 
	var random_y = randi_range(1, 18)
	
	var new_pos = Vector2((random_x * grid_size) + 20, (random_y * grid_size) + 20)
	
	return new_pos
	
