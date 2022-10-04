function decodeUplink(input) {
  if (input.bytes.length == 26)
  {
    var dt = (input.bytes[0] << 24) | (input.bytes[1] << 16) | (input.bytes[2] << 8) | (input.bytes[3]);
    var id = (input.bytes[4] << 24) | (input.bytes[5] << 16) | (input.bytes[6] << 8) | (input.bytes[7]);
    var tagid = (input.bytes[8] << 24) | (input.bytes[9] << 16) | (input.bytes[10] << 8) | (input.bytes[11]);

    var lqi_mode1 = input.bytes[12];
    var lqi_mode2 = input.bytes[13];

    var distance_mode1 = (input.bytes[14] << 24) | (input.bytes[15] << 16) | (input.bytes[16] << 8) | (input.bytes[17]);
    var distance_mode2 = (input.bytes[18] << 24) | (input.bytes[19] << 16) | (input.bytes[20] << 8) | (input.bytes[21]);
    
    var readingID = (input.bytes[22] << 24) | (input.bytes[23] << 16) | (input.bytes[24] << 8) | (input.bytes[25]);

    return {
      data: {
        readingID: readingID,
        dt: dt,
        id: id,
        tagid: tagid,

        lqi_mode1: lqi_mode1,
        lqi_mode2: lqi_mode2,

        distance_mode1: distance_mode1,
        distance_mode2: distance_mode2,

        raw: input.bytes
      },
      warnings: [],
      errors: []
    };
  }
  else
  {
    return {
      data: {
        raw: input.bytes
      },
      warnings: ["Unrecognized packet format!"],
      errors: []
    };
  }
}