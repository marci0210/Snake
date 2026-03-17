extends Control

@export var scenes_path : String = "res://scenes/homepage.tscn"

func _ready() -> void:
	get_parent().get_node("Label2").text = "Max score: " + str(Global.overall_high_score)

func _on_quit_button_pressed():
	get_tree().quit()

func _on_start_button_pressed() -> void:
	get_tree().change_scene_to_file("res://scenes/game.tscn")
