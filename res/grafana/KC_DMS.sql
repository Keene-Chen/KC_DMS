SELECT datetime as "time", msg ->> '$.temp' as temp, msg -> '$.humi' as humi, msg ->> '$.status' as status
FROM emqx.test
LIMIT 50;


# dht11
select cast(json_unquote(json_extract(payload, '$.temp')) as decimal(10, 0))   AS "temp",
       cast(json_unquote(json_extract(payload, '$.humi')) as decimal(10, 0))   AS "humi",
       cast(json_unquote(json_extract(payload, '$.status')) as decimal(10, 0)) AS "status",
       created_at                                                              AS "time"
from emqx_messages
where topic = "dht11"
order by id desc
limit 10000;

# light
select cast(json_unquote(json_extract(payload, '$.raw')) as decimal(10, 0))     AS "raw",
       cast(json_unquote(json_extract(payload, '$.light')) as decimal(10, 0))   AS "light",
       cast(json_unquote(json_extract(payload, '$.voltage')) as decimal(10, 0)) AS "voltage",
       cast(json_unquote(json_extract(payload, '$.status')) as decimal(10, 0))  AS "status",
       unix_timestamp(created_at)                                               AS "time_sec"
from emqx_messages
where topic = "light"
order by id desc
limit 10000;

# icm20608
select cast(json_unquote(json_extract(payload, '$.temp')) as decimal(10, 0))      AS "temp",
       cast(json_unquote(json_extract(payload, '$."gyro.x"')) as decimal(10, 0))  AS "gyro.x",
       cast(json_unquote(json_extract(payload, '$."gyro.y"')) as decimal(10, 0))  AS "gyro.y",
       cast(json_unquote(json_extract(payload, '$."gyro.z"')) as decimal(10, 0))  AS "gyro.z",
       cast(json_unquote(json_extract(payload, '$."accel.x"')) as decimal(10, 0)) AS "accel.x",
       cast(json_unquote(json_extract(payload, '$."accel.y"')) as decimal(10, 0)) AS "accel.y",
       cast(json_unquote(json_extract(payload, '$."accel.z"')) as decimal(10, 0)) AS "accel.z",
       cast(json_unquote(json_extract(payload, '$.status')) as decimal(10, 0))    AS "status",
       created_at                                                                 AS "time"
from emqx_messages
where topic = "icm20608"
order by id desc
limit 10000;

# ap3216c
select cast(json_unquote(json_extract(payload, '$.ir')) as decimal(10, 0))     AS "ir",
       cast(json_unquote(json_extract(payload, '$.ps')) as decimal(10, 0))     AS "ps",
       cast(json_unquote(json_extract(payload, '$.als')) as decimal(10, 0))    AS "als",
       cast(json_unquote(json_extract(payload, '$.status')) as decimal(10, 0)) AS "status",
       created_at                                                              AS "time"
from emqx_messages
where topic = "ap3216c"
order by id desc
limit 10000;

# fire
select cast(json_unquote(json_extract(payload, '$.raw')) as decimal(10, 0))     AS "raw",
       cast(json_unquote(json_extract(payload, '$.voltage')) as decimal(10, 0)) AS "voltage",
       cast(json_unquote(json_extract(payload, '$.status')) as decimal(10, 0))  AS "status",
       unix_timestamp(created_at)                                               AS "time_sec"
from emqx_messages
where topic = "fire"
order by id desc
limit 10000;

# led
select cast(json_unquote(json_extract(payload, '$.redled_status')) as decimal(10, 0))   AS "redled_status",
       cast(json_unquote(json_extract(payload, '$.greenled_status')) as decimal(10, 0)) AS "greenled_status",
       cast(json_unquote(json_extract(payload, '$.blueled_status')) as decimal(10, 0))  AS "blueled_status",
       unix_timestamp(created_at)                                                       AS "time_sec"
from emqx_messages
where topic = "led"
order by id desc
limit 10000;

# beep
select cast(json_unquote(json_extract(payload, '$.status')) as decimal(10, 0)) AS "status",
       unix_timestamp(created_at)                                              AS "time_sec"
from emqx_messages
where topic = "beep"
order by id desc
limit 10000;