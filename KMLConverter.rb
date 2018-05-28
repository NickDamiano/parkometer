require 'nokogiri'
require 'pry-byebug'

arduino_array = []

# Opens POIs.kml file in this directory, removes namespaces, extracts placemarks which is the area
	# that holds the POI info, iterates through to create proper arduino data structure
	# of nested arrays

doc = File.open("POIs.kml") { |f| Nokogiri::XML(f)}
doc.remove_namespaces!
placemarks = doc.xpath("//Folder/Placemark")
placemarks.each do | placemark |
	name = placemark.search("name").text
	lat = placemark.search('coordinates').text.split("\n")[1].split(',')[1]
	lon = placemark.search('coordinates').text.split("\n")[1].split(',').first.lstrip!
	arr = [name, lat, lon]
	arduino_array.push(arr)
end

number_of_points = arduino_array.length

output = File.open("points.txt", "w") do | line | 
	line.puts "Poi poi_list[" + arduino_array.length.to_s + "] = {"
	arduino_array.each do | point | 
		line.puts "{" + "\""  + point[0].rstrip + "\"" + "," +  point[1].strip + "," + point[2].strip + "}"
		if arduino_array[-1] != point
			line.puts ","
		end
	end
	line.puts "};"
	number_of_points = arduino_array.length
	line.puts "int number_of_points = " + number_of_points.to_s + ";"
end