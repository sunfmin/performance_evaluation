require "mongo"
require "json"

conf = JSON.parse File.read File.dirname(__FILE__) + '/config.json'
host, db, docs = conf['database'].split '/'
backend = Mongo::Connection.new(host).db(db)[docs]
server = conf['server']
resp = [200, {'Content-Type' => 'text', 'Content-Length' => '0'}, ['']]

app = lambda do |env|
  if env['HTTP_HOST'].downcase == server
    backend.insert "referer" => env['HTTP_REFERER'], "remoteAddr" => env['REMOTE_ADDR'], "path" => env['PATH_INFO']
  end
  resp
end

run app
