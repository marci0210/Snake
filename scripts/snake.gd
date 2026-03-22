extends CharacterBody2D

@export var tile_map: TileMapLayer
@export var body_segment_scene: PackedScene

enum object_types {
	FRUIT,
	BOMB
}

var body_segments: Array = []

var grid_size = 40
var move_delay = 0.5
var time_until_next_move = 0.0

var current_direction = Vector2.RIGHT
var new_target_direction = Vector2.ZERO

var is_moving = true
var prev_time_stamp = 0

@onready var current_score = get_parent().get_node("CanvasLayer/Score")

@onready var design = $Sprite2D_head

@onready var curve_texture = preload("res://assets/textures/snake_curve.png")
@onready var body_texture = preload("res://assets/textures/snake_body.png")

@onready var fruits_texture : Array = [
	preload("res://assets/textures/apple.png"),
	preload("res://assets/textures/banana.png"),
	preload("res://assets/textures/cherry.png"),
	preload("res://assets/textures/pineapple.png")
]

func _ready():
	$RayCast2D.target_position = current_direction * grid_size
	$RayCast2D.force_raycast_update()
	
	body_segments.append(self)
	for i in range(1, 6):
		add_new_segments()

func add_new_segments() -> void:
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
		
		# in case of collision with wall, itself or fruit
		var object_picked_up = false
		$RayCast2D.force_raycast_update()
		if $RayCast2D.is_colliding():
			var collider = $RayCast2D.get_collider()
			
			# wall or snake itself
			if collider is TileMapLayer or $RayCast2D.get_collider().collision_layer == 1:
				Global.update_overall_high_score(int(current_score.text))
				
				Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
				get_tree().change_scene_to_file("res://scenes/homepage.tscn")
			# object
			elif $RayCast2D.get_collider().collision_layer == 2:
				object_picked_up = true
				pick_up_object()
				
				# add new segment to the Snake
				add_new_segments()
		
		# then move the segments
		move_to_grid(current_direction)
		
		if object_picked_up:
			generate_new_object(object_types.FRUIT)
			# generate a bomb
			if int(current_score.text) >= 15:
				generate_new_object(object_types.BOMB)
				
		# increase speed
		if int(current_score.text) < 5:
			move_delay = 0.5
		elif int(current_score.text) < 10:
			move_delay = 0.4
		# at 15, generate a bomb
		elif int(current_score.text) < 20:
			move_delay = 0.3
		elif int(current_score.text) < 25:
			move_delay = 0.2
		else:
			move_delay = 0.1
		
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
	current_score.text = str(int(current_score.text) + 1)
	
func generate_new_object(type) -> void:
	if type == object_types.FRUIT:
		# generate new coordiante to the fruit
		var fruit = get_parent().get_node("FruitArea2D")
		fruit.position = generate_random_grid_position()
		# generate random texture
		var fruit_number = randi_range(0, 3)
		fruit.get_node("Sprite2D").set_texture(fruits_texture[fruit_number])
	elif type == object_types.BOMB:
		# generate new coordiante to the bomb
		var bomb = get_parent().get_node("BombArea2D")
		bomb.position = generate_random_grid_position()
	
func generate_random_grid_position():
	var position_free : bool = false
	while not position_free:
		var random_x = randi_range(1, 18) 
		var random_y = randi_range(1, 18)
		
		var new_pos = Vector2((random_x * grid_size) + 20, (random_y * grid_size) + 20)
		position_free = is_cell_vacant(new_pos)
		
		if position_free:
			return new_pos
	
func is_cell_vacant(grid_pos: Vector2i) -> bool:
	var tile_map_layer = get_parent().get_node("TileMapLayer")
	var target_map_pos = tile_map_layer.local_to_map(Vector2(grid_pos))
	
	for segment in body_segments:
		if tile_map_layer.local_to_map(segment.position) == target_map_pos:
			return false
			
	var bomb = get_parent().get_node("BombArea2D")
	if tile_map_layer.local_to_map(bomb.position) == target_map_pos:
		return false
		
	var fruit = get_parent().get_node("FruitArea2D")
	if tile_map_layer.local_to_map(fruit.position) == target_map_pos:
		return false
			

	return true
