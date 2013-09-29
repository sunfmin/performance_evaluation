stub data:

    $ mongo_console
    >>> db = CLIENT.db 'nodehello'
    >>> 10.times{ db['users'].insert name: 'Alice', bio: 'is micro and soft' }
