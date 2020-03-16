// [%raw "require('isomorphic-fetch')"]     // only for node / commonjs

let getNewPosts = () : Js.Promise.t(list(Reddit.redditPost)) =>
  Fetch.fetch(Reddit.subredditURL)
  |> Js.Promise.then_(Fetch.Response.text)
  |> Js.Promise.then_(text =>
       switch (Reddit.parseRedditResponse(text)) {
       | json => Js.Promise.resolve(json.data.children)
       | exception e => Js.Promise.reject(e)
       }
     );

let getRandomRedditPost = () =>
    getNewPosts() |>
    Js.Promise.then_(posts =>
        List.nth(posts, Random.int(List.length(posts))) |> Js.Promise.resolve
    );

let rec getRandomImagePost = () : Js.Promise.t(ref(string)) =>
    getRandomRedditPost() |> Js.Promise.then_((post: Reddit.redditPost) => {
        let data = ref("");
        if (!post.data.is_self && !post.data.media) {
            data := post.data.url;
            Js.Promise.resolve(data);
        }
        else {
            getRandomImagePost();
        }
    });

// let getUrl = () => getRandomImagePost() |> Js.Promise.then_(value => {Js.log(value); Js.Promise.resolve();});
