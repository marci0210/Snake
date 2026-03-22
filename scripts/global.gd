extends Node
const SAVE_PATH = "user://highscore.json"

var overall_high_score = 0

func update_overall_high_score(current_score: int) -> void:
	if Global.overall_high_score < current_score:
		Global.overall_high_score = current_score
		
		# save it to a file
		var data = {
			"max_score": current_score
		}
		
		var file = FileAccess.open(SAVE_PATH, FileAccess.WRITE)
		if file:
			var json_string = JSON.stringify(data)
			file.store_string(json_string)
			file.close()
			
func _ready():
	load_highscore()
	
func load_highscore():
	if not FileAccess.file_exists(SAVE_PATH):
		overall_high_score = 0
	
	var file = FileAccess.open(SAVE_PATH, FileAccess.READ)
	if file:
		var json_string = file.get_as_text()
		file.close()
		
		var json = JSON.new()
		var parse_result = json.parse(json_string)
		
		if parse_result == OK:
			var data = json.get_data()
			overall_high_score = int(data.get("max_score", 0))
	else:
		overall_high_score = 0
