import { useEffect, useState } from "react"
import Header from "./layouts/Header.tsx"
import EditorPage from "./pages/EditorPage.tsx"
import ProjectPage from "./pages/ProjectPage.tsx"

function getPathname() {
  return window.location.pathname.replace(/\/$/, "") || "/"
}

function App() {
  const [path, setPath] = useState(getPathname)

  useEffect(() => {
    const handlePopState = () => {
      setPath(getPathname())
    }

    window.addEventListener("popstate", handlePopState)

    return () => window.removeEventListener("popstate", handlePopState)
  }, [])

  useEffect(() => {
    document.title = path === "/editor" ? "Project DMX - Editor" : "Project DMX - Projects"
  }, [path])

  const navigate = (nextPath) => {
    const normalizedPath = nextPath.replace(/\/$/, "") || "/"

    if (normalizedPath === path) {
      return
    }

    window.history.pushState({}, "", normalizedPath)
    setPath(normalizedPath)
  }

  const page = path === "/editor" ? <EditorPage /> : <ProjectPage />

  return (
    <>
      <Header currentPath={path} onNavigate={navigate} />

      {page}
    </>
  )
}

export default App
