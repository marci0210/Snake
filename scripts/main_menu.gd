extends Control

@export var scenes_path : String = "res://scenes/homepage.tscn"

func _on_quit_button_pressed():
	get_tree().quit()


func _on_start_button_pressed() -> void:
	get_tree().change_scene_to_file("res://scenes/game.tscn")
