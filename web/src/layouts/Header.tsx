import logoImg from "@assets/logo.png"
import EditorMenubar from "./EditorMenubar"


export default function Header() {

  return (
    <div className="w-full h-16 bg-black">

        {/* Title */}
        <div className="flex items-center gap-4">

            <div>
                <img src={logoImg} alt="ProjectDMX Logo" />
            </div>

            <div>
                <h3>Project Name</h3>

                <div>

                    <EditorMenubar/>

                </div>

            </div>

        </div>

    </div>
  )
}