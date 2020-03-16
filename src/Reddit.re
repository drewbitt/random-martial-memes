// https://github.com/idkjs/reason-discord-bot/blob/330e0706b3389c738a32ac20cc0bac7990ece20c/src/comfy-bot/Reddit.re

let subredditName = "MartialMemes";
let subredditURL =
  "https://www.reddit.com/r/"
  ++ subredditName
  ++ "/new.json?raw_json=1&sort=new";

type redditPostData = {
  url: string,
  is_self: bool,
  media: bool,
};

type redditPost = {
  data: redditPostData,
  kind: string,
};

type redditResponseData = {
  after: string,
  before: option(string),
  children: list(redditPost),
  modhash: string,
};

type redditResponse = {
  data: redditResponseData,
  kind: string,
};

let decodeRedditPostData = json =>
  Json.Decode.{
    url: json |> field("url", string),
    is_self: json |> field("is_self", bool),
    media: json |> field("media", bool),
  };

let decodeRedditPost = (json): redditPost =>
  Json.Decode.{
    data: json |> field("data", decodeRedditPostData),
    kind: json |> field("kind", string),
  };

let decodeRedditResponseData = (json): redditResponseData =>
  Json.Decode.{
    after: json |> field("after", string),
    before: json |> optional(field("before", string)),
    children: json |> field("children", list(decodeRedditPost)),
    modhash: json |> field("modhash", string),
  };

let decodeRedditResponse = (json): redditResponse =>
  Json.Decode.{
    data: json |> field("data", decodeRedditResponseData),
    kind: json |> field("kind", string),
  };

let parseRedditResponse = (json): redditResponse =>
  json |> Json.parseOrRaise |> decodeRedditResponse;
