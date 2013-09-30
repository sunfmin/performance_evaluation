stub data (10 records):

    $ mongo_console
    >>> users = CLIENT.db('nodehello')['users']
    >>> 10.times{ users.insert name: 'Alice', bio: 'is micro and soft' }

add to 100 records:

    90.times{ users.insert name: 'Bob', bio: 'Doesnt wash face'}

remove 90 records:

    users.remove name: 'Bob'
