[%raw "require('isomorphic-fetch')"]

/* Js.log("Hello, BuckleScript and Reason!"); */

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

let rec getRandomImagePost = () =>
    getRandomRedditPost() |>
    Js.Promise.then_((post: Reddit.redditPost) => {
        if(!post.data.is_self) {
            Js.log(post.data.url);
        }
        else {
            getRandomImagePost();
        }
        Js.Promise.resolve();
    })
    |> Js.Promise.catch(error => error |> Js.log |> Js.Promise.resolve)
    |> ignore;

getNewPosts()