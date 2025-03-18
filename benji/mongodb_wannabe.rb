require 'json'

# Database class
class MongoDBWannabe
	
	# constructor
	def initialize(file = "database.json") # storage destination
		@file = file # saves file name for later use
		@data = load_data # load in data from file if exists
		@index = build_index # create index for faster search
	end


	def load_data
		File.exist?(@file) ? JSON.parse(File.read(@file)) : {} # if exist == try parse, else empty hash
		rescue JSON::ParserError
		{} # if file has invalid JSON data OR file does not exist, return empty hash to avoid crashes 
	end

	def build_index
		index = {}
		@data.each do |table, records| # loop through tables in database 
		index[table] = {} # create empty hash for each table where posts will be stored
		records.each { |record| index[table][record]["id"]] = record} # record == post. save posts data in index with id as key 
		end
		index # return index
	end


	def save_data
		File.write(@file, JSON.pretty_generate(@data)) # save data to file with pretty formating
	end

	def insert(table, record)
		@data[table] ||= [] # create empty list if table does not exist
		id = next_id(table) # fetch next available id
		record["id"] = id # add id to post
		@data[table] << record # add post to table
		@index[table] ||= {} # create empty hash if index does not exist
		@index[table][id] = record # add post to index
		save_data # save data to file
		record # return newly added post
	end

	def find(table, id)
		@index[table]&.[](id) # find post by index
	end

	def all(table)
		@data[table] || [] # return all rows in table OR empty list if not exist
	end

	def update(table, id, new_data)
		record = find(table, id) # fetch post to update with find
		return nil unless record # if post not exist return nil
	
		record.merge!(new_data) # update post with new values
		save_data # save data to file
		record # return updated post
	end

	def delete(table, id)
		@data[table]&.reject! { |r| r["id"] == id } # remove post from table using id
		@index[table]&.delete(id) # remove post from index using id
		save_data # save changes to file
	end

	def next_id(table)
		# create list of all id's in table with map. .max == get highest/latest ID
		(@data[table]&.map { |r| r["id"] }&.max || 0) + 1  # get highest available id, if table == empty start at 1.
	end


